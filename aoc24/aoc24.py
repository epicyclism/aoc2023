from itertools import combinations
from z3 import *

FILENAME = "input.txt"
MIN_POS = 200000000000000
MAX_POS = 400000000000000


def parse_input(filename):
    with open(filename, "r") as input_file:
        return [
            tuple(
                [
                    tuple([int(num) for num in nums.split(",")])
                    for nums in row.split("@")
                ]
            )
            for row in input_file.read().split("\n")
        ]


def calculate_slope_interception(hailstone):
    (x, y, _), (dx, dy, _) = hailstone
    slope = dy / dx
    intercept = y - x * dy / dx
    return slope, intercept


def check_collision_path(equations, hailstone1, hailstone2, mn, mx):
    k1, n1 = equations[hailstone1]
    k2, n2 = equations[hailstone2]
    if k1 == k2:
        return False
    x = (n2 - n1) / (k1 - k2)
    y = k1 * x + n1
    time1 = (x - hailstone1[0][0]) / hailstone1[1][0]
    time2 = (x - hailstone2[0][0]) / hailstone2[1][0]
    return mn <= x <= mx and mn <= y <= mx and time1 >= 0 and time2 >= 0


def part_one(equations, mn, mx):
    return sum(check_collision_path(equations, h1, h2, mn, mx)
               for h1, h2 in combinations(equations.keys(), 2))


def part_two(data):
    (x_1, y_1, z_1), (dx_1, dy_1, dz_1) = data[0]
    (x_2, y_2, z_2), (dx_2, dy_2, dz_2) = data[1]
    (x_3, y_3, z_3), (dx_3, dy_3, dz_3) = data[2]

    x, y, z = Reals("x y z")
    dx, dy, dz = Reals("dx dy dz")
    t1, t2, t3 = Reals("t1 t2 t3")
    solver = Solver()

    eq1 = x + t1 * dx == x_1 + t1 * dx_1
    eq2 = y + t1 * dy == y_1 + t1 * dy_1
    eq3 = z + t1 * dz == z_1 + t1 * dz_1
    eq4 = x + t2 * dx == x_2 + t2 * dx_2
    eq5 = y + t2 * dy == y_2 + t2 * dy_2
    eq6 = z + t2 * dz == z_2 + t2 * dz_2
    eq7 = x + t3 * dx == x_3 + t3 * dx_3
    eq8 = y + t3 * dy == y_3 + t3 * dy_3
    eq9 = z + t3 * dz == z_3 + t3 * dz_3

    solver.add(eq1, eq2, eq3, eq4, eq5, eq6, eq7, eq8, eq9)

    if solver.check() == sat:
        model = solver.model()
        return sum(model[var].as_long() for var in [x, y, z])


def main():
    data = parse_input(FILENAME)
    equations = {
        hailstone: calculate_slope_interception(hailstone) for hailstone in data
    }
    print(part_one(equations, MIN_POS, MAX_POS))
    print(part_two(data))


if __name__ == "__main__":
    main()
