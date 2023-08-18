const express = require("express");
const bodyParser = require("body-parser");
const fs = require("fs")
const times = require("./times.json");


const app = express();
const port = 3000;

app.use(bodyParser.json());

app.get('/api/all', function(req, res) {
  res.send(times)
});

app.get('/api/add/:time/:activate', function(req, res) {
    const time = req.params.time
    const activate = (req.params.activate).split(".")

    //let activateArray = activate.split(".") //.map((str)=>{return Number.parseInt(str)})

    times.push({time, activate})
    // console.log(activate)
    // console.log(time)
    // console.log(times)
			// Writing to a file
			// here in stringify() method null, 2 is used to append data in a indexed format
			fs.writeFile("./times.json", JSON.stringify(times, null, 2), err => {     
			    // Checking for errors
			    if (err) throw err;
			});
    res.send('time: ' + req.params.time);
});

app.get('/api/checkForAlarm', function(req, res){
  const times = require("./times.json");
  const timeObj = new Date();
  const hour = timeObj.getHours();
  const minute = timeObj.getMinutes();
  const nowTime = `${hour}:${minute}`;
   console.log("nt", nowTime);
   let flagActivate = "";

  times.forEach((time) => {
    const timeFromApi = time.time
    console.log('tfa', timeFromApi)
    if (timeFromApi == nowTime) {
      flagActivate = time.activate.join(" ")
      // break;
      // return res.send()
    } /*else {
      
    }*/    
  });

  if (flagActivate) {
    res.send(flagActivate)
  } else {
    res.send("null")
  }
});

app.listen(port, () => {
  console.log(`Server started at port http://localhost:${port}`);
});