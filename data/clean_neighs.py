import json


def polygon_line(pid, p, c1, c2):
    return f"{c1[0]} {c1[1]} {c2[0]} {c2[1]} {p[0]} {p[1]} {pid}"


def mid_point(c1, c2):
    return [(c1[0] + c2[0]) / 2, (c1[1] + c2[1]) / 2]


def main():
    with open("nyc_neighs.geojson", "r") as f:
        data = json.loads(f.read())
        for pid, feature in enumerate(data["features"]):
            coords = feature["geometry"]["coordinates"][0]
            n_coords = len(coords)
            p = mid_point(coords[0], coords[2])  # point inside polygon
            for j in range(0, n_coords - 1):
                print(polygon_line(pid, p, coords[j], coords[j + 1]))


if __name__ == "__main__":
    main()
