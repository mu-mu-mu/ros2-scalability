import sys
import glob
import re
from statistics import mean
import matplotlib.pyplot as plt


if len(sys.argv) == 1:
    print("plot.py dir1 dir2 ...")
    sys.exit(1)

for i,fname in enumerate(sys.argv[1:]):
    x = []
    y = []

    files = glob.glob(fname + "/data*")
    for f in files:
        num = int(re.search(r"\d+$",f).group())
        with open(f,"r") as fd:
            times = []
            for l in fd.readlines():
                if "Subscribed" in l:
                    times.append(int(re.search(r"\d+$",l).group()))

            if times == []:
                continue
            y.append(mean(times)/1e6)
        x.append(num)

    c= list(zip(x,y))
    c.sort()
    a,b = zip(*c)

    n = ["20%", "40%",  "60%","80%", "75%"]

    plt.plot(a,b,label="CPU : "+n[i])

plt.xticks(fontsize=19)
plt.yticks(fontsize=19)
plt.xlabel("# nodes",fontsize=22)
plt.ylabel("times (ms)", fontsize=22)
plt.legend(loc="best",fontsize=22)
plt.show()
