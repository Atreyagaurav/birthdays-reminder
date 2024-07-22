with open("dates-sorted.txt") as r:
    lines = [d.strip().split() for d in r]
    dates = {tuple(d[0].split("-")): tuple(d[1].split("-")) for d in lines}


num_days = {}

for nep, eng in dates.items():
    try:
        if num_days[(nep[0],nep[1])] < nep[2]:
            num_days[(nep[0],nep[1])] = nep[2]
    except KeyError:
        num_days[(nep[0],nep[1])] = nep[2]


years = [str(y) for y in range(2000, 2090)]
months = [f"{m:02d}" for m in range(1,13)]

for y in years:
    print(f"    {{{','.join(num_days[(y,m)] for m in months)}}},")


for y in years:
    print(y, sum(int(num_days[(y,m)]) for m in months))
