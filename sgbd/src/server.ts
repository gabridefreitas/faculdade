import "./loadEnvironment";

import express from "express";
import comments from "./routes/comments";
import home from "./routes/home";

const PORT = process.env.PORT || 5050;

const app = express();
app.use(express.json());

// Load routes
app.use("/", home);
app.use("/comments", comments);

// start the Express server
app.listen(PORT, () => {
  console.log(`Server is running on port: ${PORT} 🚀`);
});
