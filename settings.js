module.exports = {
    flowFile: 'flows.json',
    userDir: __dirname,
    httpStatic: __dirname + '/public',
    credentialSecret: false,
    functionGlobalContext: {
    },
    exportGlobalContextKeys: false,
    editorTheme: {
        projects: {
            enabled: false
        }
    }
}