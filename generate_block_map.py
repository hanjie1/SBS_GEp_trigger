print("block_coords block_map[288]={")
for chan in range(287):
    xx = chan//12
    yy = chan - xx*12
    print("{"+"{},{}".format(xx+1,yy+1)+"}",end=",")
       
chan=287
xx=chan//12
yy=chan - xx*12
print("{"+"{},{}".format(xx+1,yy+1)+"}") 
print("};")
