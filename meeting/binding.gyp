{
  "targets": [
    {
      "target_name": "NEMeetingSDK",
      "conditions": [
        ["OS == 'mac'", {
          "include_dirs": [
            "./libs/macOS/nem_hosting_module.framework/Headers"
          ],
          "libraries": [
            "-F../libs/macOS -framework nem_hosting_module"
          ]
        }],
        ["OS == 'win'", {
          "include_dirs": [
            "./libs/Windows/include"
          ],
        "copies": [{
            "destination": "build/Release",
            "files": [
              "./libs/Windows/bin/nem_hosting_module.dll"
            ]
          }],
          'msvs_settings': {
            'VCLinkerTool':
              {
                'AdditionalDependencies': ['nem_hosting_module.lib'],
                'AdditionalLibraryDirectories': ['../libs/Windows/lib'],
              }
          },
          "cflags_cc": [
            "-std=c++11",
            "-stdlib=libc++"
          ]
        }]
      ],
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "sources": [
        "./sdk/main.cc",
        "./sdk/nemeeting_manager.cc"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ]
    }
  ]
}
