import { Router } from "express"
import { readCSV } from "../services/csvService"
import path from "path"

export default function () {
    let app = Router()
    app.get("/", (req, res) => {
        res.sendFile(path.resolve(path.resolve("public/index.html")))
    })
    app.get("/data", async (req, res) => {
        let data = await readCSV()
        console.log(data)
        return res.json(data)
    })

    return app
}