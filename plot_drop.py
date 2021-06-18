import sys
import glob
import re
from statistics import mean
import matplotlib.pyplot as plt

fig, ax1 = plt.subplots()

if len(sys.argv) == 1:
    print("plot.py dir1 dir2 ...")
    sys.exit(1)

for fname in sys.argv[1:]:
    x = []
    y1 = []
    y2 = []

    files = glob.glob(fname + "/data*")
    for f in files:
        num = int(re.search(r"\d+$",f).group())
        with open(f,"r") as fd:
            res1 = []
            res2 = []
            print(f)
            for l in fd.readlines():
                if "receive errors" in l:
                    res1.append(int(re.search(r"\d+",l).group()))
                    print(res1[-1])

            if res1 == []:
                continue
            y1.append(abs(res1[0] - res1[1])/10e3)
        x.append(num)

    t1 = list(zip(x,y1))
    t1.sort()
    a1,b1 = zip(*t1)


    ll = ["0%", "25%", "50%", "60%", "75%"]
    ax1.plot(a1,b1,linestyle = "solid", label=fname+"")

#plt.xlabel("# nodes")
#plt.ylabel("times (ms)")
ax1.set_xlabel("# Nodes", size = 24)
ax1.set_ylabel("Dropped Packets (K)", size = 24)
h1, l1 = ax1.get_legend_handles_labels()
ax1.legend(h1, l1, loc='best',fontsize=20,ncol=1)
ax1.tick_params(axis='x', labelsize=22)
ax1.tick_params(axis='y', labelsize=22)
plt.show()
