var express = require("express")
//var bodyParser = require("body-parser")
var app = express()

//app.use(bodyParser.urlencoded({ extended: true }))
app.use(express.json())
app.use(express.urlencoded({ extended: false }))

app.listen(3389,function(){
    console.log("The server is running at port 3389")
})


app.all("*", function(req, res, next) {
    res.header("Access-Control-Allow-Origin", "*")
    res.header("Access-Control-Allow-Methods", "PUT, GET, POST, DELETE, OPTIONS")
    res.header("Access-Control-Allow-Headers", "X-Requested-With")
    res.header("Access-Control-Allow-Headers", "Content-Type")
    res.header("Access-Control-Allow-Credentials", "true")
    next()
})


app.get("/device/addinfo",function(req,res){
    var id = req.query.id
    var alcohol = req.query.alcohol
    var gps = req.query.gps

    res.send([id,alcohol,gps]);


})