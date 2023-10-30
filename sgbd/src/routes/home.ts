import express from "express";

const router = express.Router();

router.get("/", async (_req, res) => {
  res.json({ message: "SGBD - Atividade 8" }).status(200);
});

export default router;
