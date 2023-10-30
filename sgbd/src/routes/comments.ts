import express from "express";
import { ObjectId } from "mongodb";

import db from "../db";

const router = express.Router();

const routePath = "comments";

// Get a list of 10
router.get("/", async (_req, res) => {
  const collection = db.collection(routePath);
  const results = await collection.find({}).limit(10).toArray();

  res.send(results).status(200);
});

// Fetches the latest
router.get("/latest", async (_req, res) => {
  const collection = db.collection(routePath);
  const results = await collection
    .aggregate([
      { $project: { author: 1, title: 1, tags: 1, date: 1 } },
      { $sort: { date: -1 } },
      { $limit: 3 },
    ])
    .toArray();

  res.send(results).status(200);
});

// Get a single
router.get("/:id", async (req, res) => {
  const collection = db.collection(routePath);
  const query = { _id: new ObjectId(req.params.id) };
  const result = await collection.findOne(query);

  if (!result) res.send("Not found").status(404);
  else res.send(result).status(200);
});

// Add a new document to the collection
router.post("/", async (req, res) => {
  const collection = db.collection(routePath);

  const newDocument = req.body;
  newDocument.date = new Date();

  const result = await collection.insertOne(newDocument);

  res.send(result).status(204);
});

// Update comment
router.patch("/:id", async (req, res) => {
  const query = { _id: new ObjectId(req.params.id) };
  const updates = {
    $push: { comments: req.body },
  };

  const collection = db.collection(routePath);
  const result = await collection.updateOne(query, updates);

  res.send(result).status(200);
});

// Delete
router.delete("/:id", async (req, res) => {
  const query = { _id: new ObjectId(req.params.id) };

  const collection = db.collection(routePath);
  const result = await collection.deleteOne(query);

  res.send(result).status(200);
});

export default router;
