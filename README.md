# 开发说明
# 创建工程
    ```bash
    cocos new carrotfantasy -p com.tongji.carrotfantasy -l cpp -d e:/cocos-game
    ```
## 数据持久化
使用sqlite3数据库，若各位电脑上没有sqlite3，请自行安装。
### 安装方法：
#### 方案1：使用 vcpkg（推荐）
1. 安装 vcpkg
   ```bash
   # 1. 打开 PowerShell（管理员）
   # 2. 选择一个目录（比如 C:\dev）并执行：
   git clone https://github.com/Microsoft/vcpkg.git
   cd vcpkg
   .\bootstrap-vcpkg.bat
   ```
2. 集成到 VS2022
   ```bash
   .\vcpkg integrate install
   ```
3. 安装 sqlite3
   ```bash
   .\vcpkg install sqlite3:x86-windows
   .\vcpkg install sqlite3:x64-windows
   ```
#### 方案2：手动安装（更简单但不够优雅）
1. 下载 SQLite3
   - 访问 https://www.sqlite.org/download.html
   - 下载 sqlite-dll-win32-x86 和 sqlite-dll-win64-x64
   - 下载 sqlite-amalgamation （包含头文件）
2. 设置项目
   - 在项目目录创建 lib 和 include 文件夹
   - 将 .dll 和 .lib 文件放入 lib 文件夹
   - 将 sqlite3.h 放入 include 文件夹
3. 配置 VS2022 项目
   - 右键项目 -> 属性
   - 在 C/C++ -> 常规 -> 附加包含目录 添加 $(ProjectDir)include
   - 在 链接器 -> 常规 -> 附加库目录 添加 $(ProjectDir)lib
   - 在 链接器 -> 输入 -> 附加依赖项 添加 sqlite3.lib
4. 复制 DLL
   - 将对应版本的 sqlite3.dll 复制到项目的输出目录（通常是 Debug 或 Release 文件夹）