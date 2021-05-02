var express = require("express")
var mysql = require("mysql")
var bodyParser = require("body-parser")
var app = express()

app.use(bodyParser.urlencoded({ extended: true }))
app.use(express.json())
app.use(express.urlencoded({ extended: false }))

app.listen(3450,function(){
    console.log("The server is running at port 3450")
})


app.all("*", function(req, res, next) {
    res.header("Access-Control-Allow-Origin", "*")
    res.header("Access-Control-Allow-Methods", "PUT, GET, POST, DELETE, OPTIONS")
    res.header("Access-Control-Allow-Headers", "X-Requested-With")
    res.header("Access-Control-Allow-Headers", "Content-Type")
    res.header("Access-Control-Allow-Credentials", "true")
    next()
})

app.use("/interface",express.static('dist'))


app.get("/device/addinfo",function(req,res){


    var connection = mysql.createConnection({                //修改data数据库
        host: 'localhost',
        user: 'root',
        password: '20020118czr',
        database: 'data'
    })
    var id = req.query.id
    var alcohol = req.query.alcohol
    var gps = req.query.gps
    var instruction = "INSERT INTO data (id,alcohol,gps) VALUES(?,?,?);"
    var params = [id,alcohol,gps]
    connection.connect()
    connection.query(instruction,params,function(err,result){
        if(err){
            console.log("-------ERROR!-------")
            console.log(err.message)
        }
    })
    connection.end()


    var connection = mysql.createConnection({                   //修改device中的id 有则不管 无则插入
       host: 'localhost',
        user: 'root',
        password: '20020118czr',
        database: 'device'
    })

    var instruction3 = " insert into device (id) values(?) ON DUPLICATE KEY UPDATE id = "+id
    connection.connect()
    connection.query(instruction3,id,function(err,result){
        if(err){
            console.log("-------ERROR!-------")
            console.log(err.message)
        }
    })
    connection.end()


    var connection = mysql.createConnection({                         //修改device表
       host: 'localhost',
        user: 'root',
        password: '20020118czr',
        database: 'device'
    })

    var instruction1 = "UPDATE device SET alcohol=?, gps=? WHERE id = " + id
    connection.connect()
    connection.query(instruction1,params,function(err,result){
        if(err){
            console.log("-------ERROR!-------")
            console.log(err.message)
        }
    })
    connection.end()
})