import itertools


def _hash(votes):
        hash_bytes = [0, 0, 0, 0]
        index_iter = itertools.cycle(range(4))
        for char in votes:
            idx = next(index_iter)
            hash_bytes[idx] = (hash_bytes[idx] ^ ord(char)) & 0xFF
        return ''.join([hex(char)[2:].zfill(2) for char in hash_bytes])

print(_hash("[{\"candidate\": \"Liquid Snake\", \"count\": 1000000}]"));




#"{"checksum": "5e43472f", "precinct": "precinct-2", "votes": "[{\"candidate\": \"Liquid Snake\", \"count\": 1000000}]}"


#{"checksum": "4b056923", "precinct": "precinct-2", "votes": "[{\"candidate\": \"Liquid Snake\", \"count\": 1000000}]"}