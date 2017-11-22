import string
fp = open('test1.txt')
a = fp.read()
fp.close()

print "=================="
print a[:-1]
print "=================="

fp = open('test2.txt')
b = fp.read()
fp.close()
print "=================="
print b[:-1]
print "=================="

fp = open('test3.txt','w')
l = list(a[0:-1] + b[:-1])
l.sort()
print "=================="
print l
print "=================="
s = ''
s = s.join(l)
fp.write(s)
fp.close()
