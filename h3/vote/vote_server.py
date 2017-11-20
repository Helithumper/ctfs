import itertools
import json
import socketserver
import threading
import time

import marshmallow
import peewee
import yaml


CONF = {}
HOST = '0.0.0.0'
PORT = 31337

DB = peewee.SqliteDatabase('/home/h3/votes.db')


class Vote(peewee.Model):
    class Meta:
        database = DB
    id = peewee.PrimaryKeyField()
    candidate = peewee.TextField(null=False)
    count = peewee.IntegerField(null=False)
    ip = peewee.TextField(null=False)
    precinct = peewee.TextField(null=False)


class MsgWrapperSchema(marshmallow.Schema):
    checksum = marshmallow.fields.Str(required=True)
    precinct = marshmallow.fields.Str(required=True)
    votes = marshmallow.fields.Str(required=True)


class VoteModelSchema(marshmallow.Schema):
    candidate = marshmallow.fields.Str(required=True)
    count = marshmallow.fields.Int(required=True)


class ProtocolOne:
    def validate(self, votes, checksum):
        pass


class ProtocolTwo:
    def validate(self, votes, checksum):
        if checksum != self._hash(votes):
            raise ValueError('Invalid hash')

    def _hash(self, votes):
        hash_bytes = [0, 0, 0, 0]
        index_iter = itertools.cycle(range(4))
        for char in votes:
            idx = next(index_iter)
            hash_bytes[idx] = (hash_bytes[idx] ^ ord(char)) & 0xFF
        return ''.join([hex(char)[2:].zfill(2) for char in hash_bytes])


class ProtocolThree(ProtocolTwo):
    def __init__(self):
        with open(CONF['shared_secret_file'], 'rb') as fh:
            self.secret = fh.read().decode('latin-1', errors='ignore')[0:128]

    def validate(self, votes, checksum):
        super().validate(self.secret + votes, checksum)


class MsgParser:
    PRECINCT_MAP = {
        'precinct-1': ProtocolOne,
        'precinct-2': ProtocolTwo,
        'precinct-3': ProtocolThree,
    }

    def __init__(self, buff):
        data, errors = MsgWrapperSchema().loads(buff.decode('ascii'))
        if errors:
            raise ValueError('Invalid wrapper schema')
        if data['precinct'] not in self.PRECINCT_MAP:
            raise ValueError('No such precinct')
        self.PRECINCT_MAP[data['precinct']]().validate(
            data['votes'], data['checksum'])
        self._parse_votes(data['votes'], data['precinct'])
        self.precinct = data['precinct']

    def _parse_votes(self, votes_raw, precinct):
        votes_structured, errors = VoteModelSchema().loads(
            votes_raw, many=True)
        if errors:
            raise ValueError('Invalid vote schema')
        self.votes = {}
        for vote in votes_structured:
            if vote['count'] < 0:
                raise ValueError('Vote count must be >= 0')
            if vote['candidate'] in CONF['elections'][precinct]['candidates']:
                self.votes[vote['candidate']] = vote['count']
            else:
                raise ValueError('Invalid candidate for this precinct!')


class RequestServer(socketserver.ThreadingMixIn, socketserver.TCPServer):
    pass


class RequestHandler(socketserver.BaseRequestHandler):
    def handle(self):
        data = self.request.recv(4096)
        try:
            msg_parser = MsgParser(data)
        except ValueError as e:
            self.request.sendall(json.dumps({'msg': str(e)}).encode())
            return
        except Exception:
            return
        self._record_votes(self.client_address[0], msg_parser.precinct,
                           msg_parser.votes)
        self.request.sendall(json.dumps({'msg': 'Votes recorded'}).encode())

    def _record_votes(self, ip, precinct, votes):
        for candidate, count in votes.items():
            try:
                vote = Vote.get(Vote.ip == ip, Vote.precinct == precinct,
                                Vote.candidate == candidate)
                vote.count = vote.count + count
                vote.save()
            except Vote.DoesNotExist:
                vote = Vote(ip=ip, precinct=precinct,
                            candidate=candidate, count=count)
                vote.save()


def read_conf():
    global CONF
    with open('/home/h3/voting.yaml', 'r') as fh:
        CONF = yaml.load(fh)


def main():
    read_conf()
    server = RequestServer((HOST, PORT), RequestHandler)
    server_thread = threading.Thread(target=server.serve_forever)
    server_thread.daemon = True
    server_thread.start()
    while True:
        try:
            time.sleep(0.1)
        except KeyboardInterrupt:
            break
    server.shutdown()


if __name__ == '__main__':
    main()
