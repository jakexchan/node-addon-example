const { ipcRenderer } = require('electron')
// All of the Node.js APIs are available in the preload process.
// It has the same sandbox as a Chrome extension.
window.addEventListener('DOMContentLoaded', () => {
  const replaceText = (selector, text) => {
    const element = document.getElementById(selector)
    if (element) element.innerText = text
  }

  for (const type of ['chrome', 'node', 'electron']) {
    replaceText(`${type}-version`, process.versions[type])
  }

  ipcRenderer.send('is-initialize')
  ipcRenderer.send('is-logged-in')

  document.getElementById('initBtn').addEventListener('click', () => {
    console.info('initBtn click')
    ipcRenderer.send('initialize')
  })

  document.getElementById('loginBtn').addEventListener('click', () => {
    console.info('loginBtn click')
    const appKey = document.getElementById('appKey').value.trim()
    const accountId = document.getElementById('accountId').value.trim()
    const accountToken = document.getElementById('accountToken').value.trim()
    ipcRenderer.send('login', { appKey, accountId, accountToken })
  })

  document.getElementById('startBtn').addEventListener('click', () => {
    console.info('startBtn click')
    const nickname = document.getElementById('nickname').value.trim()
    const audio = document.getElementById('audio').checked
    const video = document.getElementById('video').checked
    ipcRenderer.send('start-meeting', { nickname, audio, video })
  })

  document.getElementById('logoutBtn').addEventListener('click', () => {
    console.info('logoutBtn click')
    ipcRenderer.send('logout')
  })

  document.getElementById('unInitBtn').addEventListener('click', () => {
    console.info('unInitBtn click')
    ipcRenderer.send('unInitialize')
  })
})

ipcRenderer.on('initialize-reply', (event, arg) => {
  console.info('initialize-reply', arg)
  const element = document.getElementById('initText')
  if (element) {
    element.innerText = arg ? 'Yes' : 'No'
  }
})

ipcRenderer.on('login-reply', (event, arg) => {
  console.info('login-reply', arg)
  const element = document.getElementById('loginText')
  if (element) {
    element.innerText = arg ? 'Yes' : 'No'
  }
})

ipcRenderer.on('start-meeting-reply', (event, arg) => {
  console.info('start-meeting-reply', arg)
  const element = document.getElementById('startText')
  if (element) {
    element.innerText = arg
  }
})

ipcRenderer.on('status-reply', (event, arg) => {
  console.info('status-reply', arg)
  const element = document.getElementById('statusText')
  if (element) {
    element.innerText = typeof arg === 'object' ? JSON.parse(arg) : ''
  }
})

ipcRenderer.on('logout-reply', (event, arg) => {
  console.info('logout-reply', arg)
  ipcRenderer.send('is-logged-in')
})

ipcRenderer.on('unInitialize-reply', (event, arg) => {
  console.info('unInitialize-reply', arg)
  ipcRenderer.send('is-initialize')
})