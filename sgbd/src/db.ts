import { MongoClient } from "mongodb";

const connectionString = process.env.ATLAS_URI || "";

const client = new MongoClient(connectionString);

const conn = (await client.connect()) satisfies MongoClient;
await client.db("sample_mflix").command({ ping: 1 });

const db = conn.db("sample_mflix");

export default db;
