import express from 'express'
import loaders from "./loaders"
import config from "./config"
import path from "path"

let startServer = async () => {

    const app = express()
    app.use(express.static(path.resolve('public')))
    await loaders({ expressApp: app })

    app.listen(config.port, () => {
        console.log("Server running in port:", config.port);
    })

}

startServer()