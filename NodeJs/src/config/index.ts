import dotenv from "dotenv"
dotenv.config()

export default {
    port: process.env.PORT,
    csvPath: process.env.DATA_PATH
}