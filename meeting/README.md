从 [NEMeeting](https://github.com/netease-im/NEMeeting/blob/master/%E5%BC%80%E5%8F%91%E6%96%87%E6%A1%A3/SDK%E9%9B%86%E6%88%90/Windows&macOS/user_guide.md) 下载 SDK 到 meeting/libs/macOS 和 meeting/libs/Windows

- 编译插件

```bash
node-gyp rebuild --target=10.1.5 -dist-url=https://atom.io/download/atom-shell
```

- 启动

```bash
npm run start
```