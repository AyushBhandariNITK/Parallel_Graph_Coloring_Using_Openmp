import numpy  as np
import matplotlib.pyplot as plt
files=['af_shell10','bone010','coPapersDBLP','nlpkkt120']
print("Graphs")
print("1. af_shell10 ")
print("2. bone010 ")
print("3. coPapersDBLP")
print("4. nlpkkt120 ")
print("5. All results")
ch=int(input("Enter the choice"))
if(ch<5):
	data=np.loadtxt('/home/ayush/Parallel_Graph_Coloring_Using_Openmp/'+files[ch-1]+'.txt')
elif (ch==5):
	for i in range (len(files)):
		data=np.loadtxt('/home/ayush/Parallel_Graph_Coloring_Using_Openmp/'+files[i]+'.txt')		
		x = data[:, 1]
		y = data[:, 0]
		plt.plot(x,y,label=files[i])
	plt.xlabel("No.of threads")
	plt.ylabel("Time taken(ms)")
	plt.legend()
	plt.show()
	exit(1)
else:
	print("Invalid choice")
	exit(1)	
x = data[:, 1]
y = data[:, 0]
plt.plot(x, y,label=files[ch-1])
plt.xlabel("No.of threads")
plt.ylabel("Time taken(ms)")
plt.legend()
plt.show()
