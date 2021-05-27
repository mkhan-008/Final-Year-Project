
export default {
    getDateStringNow() {
        let now = new Date()
        return now.getFullYear()
            + "_" + (now.getMonth() + 1)
            + "_" + now.getDate()
    }
}