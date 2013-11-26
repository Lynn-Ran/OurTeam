OurTeam
=======

Lua插件系统设计任务
此任务预计3-4个月内完成, 主要目的提高对lua的理解及相关工具内容的了解与学习  
  
		开始日期: 2013/10/18  
		结束日期: ----/--/--  
		发起人: LynnRan  
  

1, Lua基础 已完成	2013/10/25 
  
		C++内私有化    
  
2,  Lua基础 已完成	2013/11/01   
  
		Lua内私有化  未完成(原因:任务发布时未清晰阐述)  
  
		闭包与Upvalue理		

3, Lua基础 已完成	2013/11/15 
  
Git基础  
  
		Git自学网页: http://pcottle.github.io/learnGitBranching/  
		掌握: 获得版本, 向版本库添加文件, 上传文件, 修改文件等  
  
 
  
Lua内私有化  
  
		C++内向Lua注册函数fn_A, 达到在脚本直接开放接口的目的  
		1.lua 如下  
		function inner()  
			....  
		end  
   
		function outer()  
			...  
		end  
		local mode = "test"  
		fn_A(mode,outer)  

		此时在共有脚本2.lua内的可以使用plus.test.outer, 但不允许调用inner  

4, lua GC 未完成	2013/11/22

		GC流程图  Mark Color
		usedata(light&full)
		weak table
   
5, 本地工程私有化: 实现双State, 内部默认私有化  

6, 本地工程私有化: 实现C++内向公有State代理  

7, 本地工程私有化: 实现Lua内向公有State代理  

8, 本地工程私有化: 实现某一插件系统进行测试  

9, 私有化暴力测试: 在只允许写插件内容的情况下, 越权获取私有内容(可以想各种方法进行)  
