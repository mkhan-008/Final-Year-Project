import morgan from "morgan"
import fs from "fs"
import helper from "../utils/helper"
import routes from "../routes"

export default async function ({ app }) {
    console.log(helper.getDateStringNow())
    app.use(morgan("common", {
        stream: fs.createWriteStream(`./logs/access_${helper.getDateStringNow()}.log`, { flags: 'a' })
    }))
    app.use(morgan("dev"))
    app.get('/status', (req, res) => res.status(200).end("ok"))
    app.use("/", routes())
}