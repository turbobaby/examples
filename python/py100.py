keys = ['a', 'b', 'c', 'd']
vals = [1, 2, 3, 4]

d = {}

for k in range (0 , len(keys)) :
    d[keys[k]] = vals[k]

kk = d.keys();
kk.sort()
for k in range(0, len(kk)):
    print (kk[k], d[kk[k]])
