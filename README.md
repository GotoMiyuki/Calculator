🧮 Multi-Calculator — 稀疏多项式与表达式计算系统

📘 项目概述

Multi-Calculator 是一个基于 C++ 实现的多功能数学计算系统，支持 稀疏多项式运算 与 一般数学表达式求值 两大功能模块。
程序采用模块化与面向对象设计，兼容控制台与图形界面（Qt / SwiftUI）两种版本。

⸻

🧩 功能模块

🔹 1. 稀疏多项式计算器（Polynomial Calculator）

支持以下操作：

功能编号	功能描述	示例
1	输入 Polynomial 1	用户输入每一项系数与指数
2	输入 Polynomial 2	同上
3	显示 Polynomial	输出形如 3x²+2x-5
4	多项式加法	P1 + P2
5	多项式减法	P1 - P2
6	多项式乘法	P1 × P2
7	求导数	dP/dx
8	计算 P(x)	代入 x 值求结果
0	返回主菜单	退出模块

技术特点：
	•	采用单向链表存储多项式项（Term 节点）。
	•	自动合并同类项，跳过零系数。
	•	Derivative() 函数支持一阶求导。
	•	evaluate(x) 支持实数代入计算。
	•	toSuperscript() 函数将幂次格式化为上标形式，如 x²+x³-1。

⸻

🔹 2. 表达式计算器（Expression Calculator）

支持任意包含加、减、乘、除、幂和括号的算术表达式求值。

示例输入：

3 + 4 * (2 - 1)^3

功能特性：
	•	支持运算符：+, -, *, /, ^
	•	支持括号 ()
	•	运算符优先级自动处理
	•	输入以 # 结束
	•	错误检测与异常处理（如括号不匹配、除零等）

技术实现：
	•	利用 双栈法（数值栈 + 运算符栈）实现中缀表达式求值。
	•	precedence() 控制优先级。
	•	applyOp() 实现运算符逻辑。
	•	Qt 版本中可用 QRegularExpression 进行输入验证。

⸻

💡 主菜单交互设计

程序启动后显示：
```
Main Menu of Multi-Calculator
1. Sparse Polynomial Calculator
2. Expression Calculator
enter 0 to quit
==============================
enter your choice:
输入相应编号进入不同模块。
```

⸻

⚙️ 编译与运行

💻 命令行编译
```
g++ main.cpp polynomial.cpp expression.cpp -o MultiCalculator
```
▶️ 运行程序
```
./MultiCalculator
```
🧰 环境要求
	•	C++17 或更高版本
	•	支持 cmath, iostream, string
	•	（GUI 版）需安装 Qt 6+

⸻

🖥️ 图形界面版本（Qt / SwiftUI）

Qt 版本：
	•	使用 QStackedWidget 实现多页面切换（Polynomial / Expression）
	•	QPlainTextEdit、QPushButton 实现输入与控制
	•	支持清空、求值、切换、显示结果
	•	运行文件通过 .pro 项目构建

SwiftUI 版本：
	•	原生 iOS 应用结构
	•	提供触觉反馈、动画效果、蓝色选中状态
	•	多项式输入栏与操作按钮分区清晰

⸻

🔮 后续改进方向
	•	多项式输入解析器：支持直接输入 "3x^2 - x + 5"
	•	扩展函数库：支持 sin(x), log(x), exp(x)
	•	UI 优化：增加输入高亮与数学符号按钮
	•	文件 I/O：保存与读取历史计算
	•	跨平台打包：生成 .exe、.app、.dmg、WebAssembly 等版本

⸻

👨‍💻 开发者信息

作者：GotoMiyuki
语言：C++ / Swift / Qt
项目名称：Multi-Calculator
版本：v1.0 (CLI)
开发时间：2025
