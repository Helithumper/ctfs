import mechanize


br = mechanize.Browser()
br.open("http://ctf.hackucf.org:4000/calc/calc.php")


br.select_form(name="action");

print br.read()