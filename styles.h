body{
  display: flex;
  flex-direction: column;
  font-family: sans-serif;
  align-items: center;
  width: 100vw;
  height: 100vh;
  color: #ffffff;
  background-color: #1a1b22;
}
.container {
  display: flex;
  flex-direction: column;
  text-align: center;
  width: 500px;
  padding: 1rem;
  border: 1px solid #666666;
  border-radius: 10px;
}
.compass {
  display: flex;
  align-items: center;
  justify-content: center;
  width: 150px;
  height: 150px;
  margin: 2rem auto;
  background-color: rgba(255,0,255,0.2);
  border-radius: 10px;
}
#arrow{
  width: 100px;
  transform: translateX(-50%);
}
table {
  font-size: 0.8rem;
  text-align: left;
  width: 200px;
  margin: 1rem auto;
  border: 1px solid #666666;
  border-collapse: collapse;
}
td {
  padding: 0.25rem;
  margin: 0;
  border: 1px solid #666666;
}
tr:nth-child(even) {
  background-color: rgba(255,255,255,0.25);
}
.hidden {
  display: none;
}