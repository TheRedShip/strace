import requests
from bs4 import BeautifulSoup
import re

# category mapping helper
def categorize(typename):
    t = typename.strip()
    if re.match(r'struct ', t): return "STRUCT"
    if any([True for test in ["addr","start","end"] if test in t]): return "PTR"
    if re.match(r'^(unsigned|size_t|pid_t)', t): return "UINT"
    if re.match(r'^(int|long|ssize_t)', t): return "INT"
    if '*' in t and re.match(r'.*(char|const char).*\*', t): return "STR"
    if re.match(r'.*\*', t): return "PTR"
    return "INT"

entries = []
json_url = "https://syscalls.mebeim.net/db/x86/64/x64/latest/table.json"
j = requests.get(json_url)

for entry in j.json()['syscalls']:
    index = entry['index']
    name = entry['name']
    args = entry.get('signature')
    argc = len(args)
    
    cats = [categorize(a) for a in args]
    entries.append((int(index), name, argc, cats))

max_index = max([e[0] for e in entries])
info = [None] * (max_index + 1)
for index, name, argc, cats in entries:
    info[index] = (name, argc, cats)

print("static const t_syscall_info syscall_info[%d] = {" % (max_index+1))
for index, rec in enumerate(info):
    if rec:
        name, argc, cats = rec
        cat_str = "{" + ", ".join(f"{c}" for c in cats) + "}"
        print(f"    [{index}] = {{\"{name}\", {argc}, {cat_str}}},")
print("};")