module.exports = {
    flowFile: 'flows.json',
    userDir: __dirname,
    httpStatic: __dirname + '/public',
    functionGlobalContext: {
        // os:require('os')
    },
    exportGlobalContextKeys: false,
    editorTheme: {
        projects: {
            enabled: false
        }
    }
}