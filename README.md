# 数据结构与算法综合实践

大一夏季学期《数据结构与算法综合实践》课程代码，C 语言实现。

**覆盖主题**：文件读写、加密算法、树的遍历与路径求解、图与状态空间搜索、启发式搜索（IDA\*）、优化算法（TSP / 模拟退火）、迷宫生成与求解（DFS / BFS / A\* / Q-Learning / 强化学习）、算法在现实场景的应用。

## 目录结构

```
├── 01_FileIO/                  # 文件读写基础
├── 02_Encryption/              # 实践一：加密算法
├── 03_Tree/                    # 实践二、三：树的遍历与路径求解
├── 04_Search/                  # 实践四：图与状态空间搜索
├── 05_HeuristicSearch/         # 实践五：启发式搜索
├── 06_Optimization/            # 实践六：优化算法
├── 07_Maze/                    # 迷宫生成与多种算法求解
├── 08_Application/             # 迷宫扩展应用
├── README.md                   # 项目说明
├── .gitignore                  # 忽略编译产物 (*.exe)
└── min_heap_core_operations.docx  # 最小堆学习笔记
```

## 01_FileIO 文件读写基础

| 文件 | 说明 |
|---|---|
| `file_write.c` | 逐字符从键盘读入，遇 `$` 停止并写入 `file.txt` |
| `file_read.c` | 从 `file.txt` 逐字符读取，统计指定字符出现次数 |
| `file_create.c` | 用 `fputs` 将字符串写入文件 |
| `file_binary_fseek.c` | 二进制文件读写 + `fseek` 定位读取 |
| `file_formatted_fseek.c` | 格式化写入 1~10，再用 `fseek` + `fscanf` 随机访问 |

## 02_Encryption 加密算法（实践一）

| 文件 | 说明 |
|---|---|
| `practice1_caesar_cipher.c` | 凯撒密码（字母环形移位），对文件内容加密/解密 |
| `practice1_multi_layer_crypto.c` | 多层复合加密：256 模字符移位 + XOR 异或，逐层迭代累加（层数生效），加解密互为逆过程 |

## 03_Tree 树（实践二、三）

树均采用**孩子-兄弟表示法**（`firstChild` / `nextSibling`），节点带 `parent` 指针。

| 文件 | 说明 |
|---|---|
| `practice2_tree_contains.c` | 递归 DFS 判断节点在树中是否存在 |
| `practice2_tree_dfs_path.c` | 递归先序遍历，输出 DFS 访问路径 |
| `practice3_tree_path_explicit_stack.c` | 求根到目标节点的路径：`findNode` + 沿 `parent` 回溯 + 反转数组（显式栈） |
| `practice3_tree_path_implicit_stack.c` | 同一问题，纯递归单趟构建正序路径（隐式栈），与上者对比 |

## 04_Search 图与状态空间搜索（实践四）

| 文件 | 说明 |
|---|---|
| `practice4_graph_path.c` | 邻接表图，递归 DFS 与显式栈 DFS 求起点→终点路径 |
| `practice4_puzzle_dfs.c` | 8-Puzzle（九宫格）盲搜：状态空间 DFS，含剪枝与环路检测 |
| `practice4_maze.c` | 10×10 迷宫，递归 DFS 与栈式 DFS 对比 |
| `practice4_missionaries_cannibals.c` | 传教士与野人过河问题，DFS + 状态合法性校验 + 链表去重 |

## 05_HeuristicSearch 启发式搜索（实践五）

| 文件 | 说明 |
|---|---|
| `practice5_puzzle_idastar.c` | 8-Puzzle 的 **IDA\*** 求解，启发式为曼哈顿距离，`f=g+h` 剪枝 + 迭代加深 |

## 06_Optimization 优化算法（实践六）

| 文件 | 说明 |
|---|---|
| `practice6_tsp.c` | 10 城旅行商问题：随机初始化 + 局部搜索（hill climbing），自实现牛顿迭代开方 |
| `practice6_probabilistic_search.c` | 模拟退火思想求解 Rastrigin 多峰函数极值（概率接受劣解 + 步长衰减） |

## 07_Maze 迷宫生成与求解算法

同一 10×10 迷宫（起点 (0,0)，终点 (9,9)），用多种算法求解，可横向对比：

| 文件 | 算法 | 特点 |
|---|---|---|
| `maze_generator.c` | — | 按难度生成迷宫：简单 / 复杂 / 不可解（整行墙分隔） |
| `maze_dfs.c` | DFS | 显式栈，栈即路径，无路回溯 |
| `maze_bfs.c` | BFS | 队列逐层扩展 + `parent` 回溯，保证最短路径 |
| `maze_dfs_memory.c` | 记忆 DFS | 成功后把正确方向记入链表，模拟老鼠记忆 |
| `maze_astar_oneway.c` | 单向 A\* | 曼哈顿启发 + 优先队列 |
| `maze_astar_twoway.c` | 双向 A\* | 起终点同时搜索，相遇点拼接 |
| `maze_astar_compare.c` | 单/双向 A\* | 同一程序对比两种 A\* 的实现 |
| `maze_qlearning.c` | Q-Learning | 强化学习：epsilon-greedy，Q 表 `[行][列][方向]`，1000 episode 训练 |
| `maze_heuristic_rl.c` | 启发式强化 | 用欧氏距离初始化 Q 表 + epsilon 动态衰减，加速收敛 |

> 所有迷宫程序的路径可视化格式统一为：`0` 表示通路，`#` 表示墙，`*` 表示路径。

## 08_Application 应用扩展

| 文件 | 算法 | 场景 |
|---|---|---|
| `maze_extension_roadmap_dijkstra.c` | Dijkstra | 迷宫/地图路径规划：自驾 / 步行两种模式，道路类型权重调整 |
| `maze_extension_gas_pipeline.c` | A\* | 西气东输管线规划：综合建设成本 + 维护成本 + 泵站成本 |

## 算法/数据结构总览

| 类别 | 覆盖 |
|---|---|
| 搜索 | DFS（递归/显式栈）、BFS、A\*、双向 A\*、IDA\*、Dijkstra |
| 树 | 孩子-兄弟表示法、先序遍历、parent 回溯、显式/隐式栈对比 |
| 图 | 邻接表、状态空间建模、环路检测 |
| 优化 | 局部搜索、模拟退火（概率接受）、TSP |
| 机器学习 | Q-Learning、启发式初始化强化学习 |
| 数据结构 | 栈、循环队列、优先队列（插入排序）、链表、动态数组 |
| 其他 | 凯撒密码、XOR 加密、文件随机访问 |

## 编译运行

Windows 下使用 MinGW：

```bash
gcc 文件名.c -o 文件名.exe
```

示例：

```bash
gcc 07_Maze/maze_astar_compare.c -o 07_Maze/maze_astar_compare.exe
./07_Maze/maze_astar_compare.exe
```

**注意**：源码为 UTF-8 编码，Windows 控制台默认是 GBK 代码页，若编译运行后中文输出乱码，先执行 `chcp 65001` 再运行，或直接在 VSCode 终端 / Windows Terminal 中运行。

## 数据文件说明

程序运行过程中会自动生成或读取以下数据文件：

### 程序自动生成（无需预先准备）

| 文件 | 内容 | 生成程序 |
|---|---|---|
| `original.txt` | 明文测试文本 `Hello World! 12345` | `practice1_caesar_cipher.c` |
| `encrypted.txt` | 明文经凯撒移位 4 位后的密文 | `practice1_caesar_cipher.c` |
| `decrypted.txt` | 密文解密后的结果（与明文一致） | `practice1_caesar_cipher.c` |
| `data.dat` | 4 个 int 的二进制数据 `{1,2,3,4}` | `file_binary_fseek.c` |
| `abc` | 格式化写入的 1~10 | `file_formatted_fseek.c` |
| `file.txt` | 键盘输入写入的字符序列 | `file_write.c` / `file_create.c` |

### 需要预先存在的输入文件

| 文件 | 被谁读取 | 如何获得 |
|---|---|---|
| `file.txt` | `file_read.c`（统计指定字符出现次数） | 先运行 `file_write.c` 或 `file_create.c` 生成，或手动创建 |

> `practice1_multi_layer_crypto.c` 为交互式程序，需要你指定一个已存在的输入文件（例如 `original.txt`）。

### 运行流程

涉及数据文件的程序按依赖关系可分为三组：

**1. `file.txt` 的生成与使用**（必须先生成，再读取）

| 程序 | 角色 | 说明 |
|---|---|---|
| `file_create.c` | 生成 | 无需交互，直接写入固定字符串 `fopen example` |
| `file_write.c` | 生成 | 交互式，键盘逐字符输入，遇 `$` 停止写入 |
| `file_read.c` | 读取 | 统计 `file.txt` 中字符 `'d'` 的出现次数，**需先运行上面任一程序生成 `file.txt`** |

**2. 自给自足，直接运行**（自动生成并读写自己的数据文件）

| 程序 | 生成文件 | 行为 |
|---|---|---|
| `file_binary_fseek.c` | `data.dat` | 写入 4 个 int `{1,2,3,4}`，再从末尾倒序定位读取第 2 个 int 输出 |
| `file_formatted_fseek.c` | `abc` | 写入 1~10，再按偏移随机读回输出 |

**3. 加密流程**

1. 先运行 `practice1_caesar_cipher.c`，自动生成 `original.txt`（明文）→ `encrypted.txt`（密文）→ `decrypted.txt`（解密结果），一条龙无需交互；
2. 再运行 `practice1_multi_layer_crypto.c`（交互式），输入一个已存在的文件（如 `original.txt` / `encrypted.txt`）、移位密钥、异或密钥、层数、加/解密标志，输出到新文件。解密需使用与加密相同的密钥与层数。
