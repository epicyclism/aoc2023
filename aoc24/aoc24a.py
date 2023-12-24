import re, z3, math, collections, itertools as it

Stone = collections.namedtuple("Stone", ["x", "y", "z", "dx", "dy", "dz"])

def parse(fn):
    return [Stone(*map(int, re.findall(r'-?\d+', l))) for l in open(fn).readlines()]

def part2(hail):
    x, y, z, dx, dy, dz = map(z3.Int, ["x", "y", "z", "dx", "dy", "dz"])
    s = z3.Solver()
    for i, h in enumerate(hail):
        t = z3.Int(f"t{i}")
        s.add(x + dx*t == h.x+h.dx*t)
        s.add(y + dy*t == h.y+h.dy*t)
        s.add(z + dz*t == h.z+h.dz*t)
    
    s.check()
    print(s.model().eval(x+y+z))

hail = parse("input.txt")
part2(hail)
