# Smartcar-on-Mount-Haruna
2018第十三届东南大学智能车校赛比完赛晚上秋名山见队代码

本仓库横跨两年校赛，两年校赛赛道元素不同。

目前除[reconstructed](https://github.com/Sciroccogti/Smartcar-on-Mount-Haruna/tree/reconstructed)分支外，均为第一年（十三届）校赛使用的代码分支。

[reconstructed](https://github.com/Sciroccogti/Smartcar-on-Mount-Haruna/tree/reconstructed)分支借鉴了[淼哥代码](https://github.com/mythbuster5/CSMS-SMARTCAR)

## 结合VScode

1. 安装[IAR Embedded Workbench 插件](https://marketplace.visualstudio.com/items?itemName=politoleo.iar)
2. 用 *VScode* 打开 `smartcar.code-workspace`
3. 启用插件，并在`.vscode`文件夹下新建`iar.json`，填写内容如下（`Path`为你的 *IAR* 安装目录）：
    ```json
    {
    "version": 1,
    "path": "C:\\Program Files (x86)\\IAR Systems\\Embedded Workbench 8.0\\",
    "project": "${workspaceFolder}\\KEA Demo\\IAR\\SEUsmartcar_KEA.ewp",
    "config": "Debug"
    }
    ```
4. 使用`Ctrl+Shift+B`编译。