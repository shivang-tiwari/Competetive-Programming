https://codeforces.com/blog/entry/149273


Define the State Vector ($V_k$)

$$V_k = \begin{pmatrix} DP[k] \\ 
DP[k+1] \\ 
\vdots \\ 
DP[k+C-1] 
\end{pmatrix}$$

The goal is to find a $C \times C$ transition matrix $M$ such that:

$$V_{k+1} = M \cdot V_k$$

Your DP state is defined over a sliding window of size $C$, where $C$ is the dependency depth.

Let the DP recurrence be:

$$DP[i] = \sum_{j=1}^{C} \alpha_j \cdot DP[i-j]$$
 
Then we have
 
$$M_{i, j} = \begin{cases} 
     1 & \text{if } j = i+1, i = 0, \cdots, C-2\\ 
     0 & \text{if } j \ne i+1,  i = 0, \cdots, C-2\\
     \alpha_{j} & \text{if } i = C-1
     \end{cases} $$

