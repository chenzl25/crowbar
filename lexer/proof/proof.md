##DFA状态数目最小化算法理解及可行性证明

####理解
何为最小化状态数目？
我们既然要最小化状态的数目，那么为什么会有状态的多余？
根据子集构造法（subset construction）我们可以发现龙书上子集构造法那条if语句的判断是假定了DFA状态节点的相同是通过给定的NFA状态节点集合来判定的，这里就是DFA状态多余的由来，因为给定的DFA转态节点的相同时不能够单凭NFA状态节点集合来判定的。有些NFA状态节点集合即使不相同，但它们的输入和输出相同的话也可以认为相同。
既然有的转态的多余我们如何区分？
给出区分DFA状态s，t的定义：**存在一个string使得：s，t其中只有一个达到了accept状态，另外一个没达到。**
我们知道DFA中的每个状态转移都是确定的，那么给定一个状态一个string，我们就能一直运行下去。看最后能否达到accept状态。
如果我们最后能够构造出一个和冗余的DFA所代表语言L一样的每个状态都是可区分的DFA那么我们就成功了。

给出龙书上的最小化DFA状态数目算法的步骤:

 1. 初始化两个group，一个包含所有的accept状态、另外一个不包含accept状态。记group的集合为G
 2. 对每个group进行划分，当且仅当符合以下条件的状态s和t在划分在同一组：对每个input(一个字符)它们都转移到G中相同的group。
 3. 把步骤2中所有划分过后的group组合起来形成一个新的group的集合G‘
 4. 如果G != G' 那么令G = G' 重新进入步骤2，否则进入步骤5
 5. 最终得到的G' 我们记为GF。 对GF中的每个组选出一个代表(representative) 。它们就是转态最小化的DFA了，记作D'。 原来的DFA记为D
 6. D'的开始状态S’ 为D的开始状态S所在GF中的group的代表
 7. D'的accept状态为为D的accpet状态所在GF中的group的代表
 8. D'的转移关系是：如果对D'中状态s，它原先在D‘中的对input a的转移到t，那么t在GF中的group的代表r和s会有这样的转移关系s =a=> r

####证明
我们下面证明一下这个算法的可行性

*Basic*

对最开始的分组G，空string : ε 可以区分不同组之间的状态，而同一组里则ε不可区分它们
对于第1轮迭代：任意长度为1的string : s1可以区分不同组之间的状态，而同一组里则s1不可区分它们

*Induction*

假设对于第i轮迭代：任意长度为i的string : si可以区分不同组之间的状态，而同一组里则si不可区分它们
在构建第i+1轮的G'时：根据算法的操作我们有一个input a[i+1]进来使得分裂的group所走去向不同了。那么又因为第i轮的G的不同分组是可区分的。那么我们就能得到新的string: **si + a[i+1]**使得第i+1轮的不同组之间的状态可区分，同组的对任意长度为i+1的string不能区分。

*end*

由于状态和输入input类型的有限性这个过程总会结束。结束后就是状态数目最小的了，因为它没不再可分。
那么我们证明下最终由上面算法所构建的D的语言L’和原先的D'锁代表的语言L等价。
对原来L中的所有accept string放到D'中，也是有D'中的S‘来开始，而S’是由D中S和与S不可区分的状态来组成的group的代表。对于给定的accept string，D'上跑的结果就是和D上的大同小异，只不过D'中的每个状态都是隐含着D中对应跑accept string的等价状态的集合。因为算法的转态转移关系保证了这一点。最后D accept的话D'也是会accpet的。
同理对于D中不accpet的string，D'也不会accpet
所以语言等价性证明完了。


####附：龙书上的子集构造法伪代码
```
initially ε-closure(s0) is the only state in Dstates, and it is unmarked;
while(there is an unmarked state T in Dstates) {
  mark T;
  for (each input symbol a) {
    U = ε-closure(move(T, a));
    if(U is not in Dstates)
      add U as an unmarked state to Dstates;
    Dtran[T, a] = U;
  }
}
```
