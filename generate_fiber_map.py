print("ap_uint<7> fiber_map[288]={")
for chan in range(287):
    xx = chan//12
    yy = chan - xx*12
    nx = xx//2
    ny = yy//2
    nn = nx*6 + ny
    print(nn,end=", ")
       
chan=287
xx=chan//12
yy=chan - xx*12
nx = xx//2
ny = yy//2
nn = nx*6 + ny
print(nn)
print("};")
