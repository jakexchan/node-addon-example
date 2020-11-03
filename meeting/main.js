// Modules to control application life and create native browser window
const {app, BrowserWindow, ipcRenderer} = require('electron')
const path = require('path')
const NEMeetingSDK = require('bindings')('NEMeetingSDK')
const { ipcMain } = require('electron')
const EventEmitter = require('events').EventEmitter;
const meetingInstance = new NEMeetingSDK.NEMeetingManager()

const emitter = new EventEmitter();
meetingInstance.callEmit(emitter.emit.bind(emitter))

function listeners () {
  ipcMain.on('is-initialize', function (event) {
    event.reply('initialize-reply', meetingInstance.isInitialized())
  })
  ipcMain.on('is-logged-in', function (event) {
    event.reply('login-reply', meetingInstance.isLoggedIn())
  })

  ipcMain.on('initialize', function (event) {
    meetingInstance.initialize(function () {
      event.reply('initialize-reply', meetingInstance.isInitialized())
    })
  })

  ipcMain.on('login', function (event, { appKey, accountId, accountToken }) {
    meetingInstance.login(appKey, accountId, accountToken, function () {
      event.reply('login-reply', meetingInstance.isLoggedIn())
    })
  })

  ipcMain.on('start-meeting', function (event, { nickname, audio, video }) {
    meetingInstance.startMeeting(nickname, audio, video, function ({ errorCode }) {
      event.reply('start-meeting-reply', errorCode)
    })
  })

  ipcMain.on('logout', function (event) {
    meetingInstance.logout(function ({ errorCode }) {
      event.reply('logout-reply', errorCode)
    })
  })

  ipcMain.on('unInitialize', function (event) {
    meetingInstance.unInitialize(function ({ errorCode }) {
      event.reply('unInitialize-reply', errorCode)
    })
  })
}

function createWindow () {
  // Create the browser window.
  const mainWindow = new BrowserWindow({
    width: 800,
    height: 600,
    webPreferences: {
      preload: path.join(__dirname, 'preload.js')
    }
  })

  // and load the index.html of the app.
  mainWindow.loadFile('index.html')

  listeners()
  emitter.on('status', function (response) {
    console.info('status: ', response)
    mainWindow.webContents.send('status-reply', response)
  })
  // Open the DevTools.
  mainWindow.webContents.openDevTools()
}

// This method will be called when Electron has finished
// initialization and is ready to create browser windows.
// Some APIs can only be used after this event occurs.
app.whenReady().then(() => {
  createWindow()
  
  app.on('activate', function () {
    // On macOS it's common to re-create a window in the app when the
    // dock icon is clicked and there are no other windows open.
    if (BrowserWindow.getAllWindows().length === 0) createWindow()
  })
});

// Quit when all windows are closed, except on macOS. There, it's common
// for applications and their menu bar to stay active until the user quits
// explicitly with Cmd + Q.
app.on('window-all-closed', function () {
  if (process.platform !== 'darwin') app.quit()
})

// In this file you can include the rest of your app's specific main process
// code. You can also put them in separate files and require them here.
