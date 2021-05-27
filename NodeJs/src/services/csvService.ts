import fs from "fs"
import csv from "csv-parser"
import config from "../config"

export const readCSV = async () => new Promise((resolve, reject) => {
    let results = []
    try {
        fs.createReadStream(config.csvPath)
            .pipe(csv())
            .on('data', (data) => { if (JSON.stringify(data) !== "{}") results.push(data) })
            .on('end', () => {
                resolve(results);
            });
    } catch (err) {
        reject(err)
    }

})
