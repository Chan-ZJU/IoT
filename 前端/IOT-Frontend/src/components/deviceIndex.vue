<template>
<div class="home">
  <div :style="styles"></div>
    <body></body>
    <el-container>
      <el-header style="width: 100%; margin-top: 8%; padding: 5px color: black">
        <el-row>
          <el-col :span="24">
            <span style="font-size: xx-large; font-weight: bold; color: black"
              >酒驾报警系统</span
            >
          </el-col>
        </el-row>
      </el-header>
      <el-main style="width: 100%; padding-top: 2%">
        <el-divider>
          <i class="el-icon-discover"></i>
        </el-divider>
        <el-row>
          <el-col :span="16" :offset="4">
            <span style="font-size: medium; font-weight: normal; color: black"
              ></span
            >
          </el-col>
        </el-row>
        <el-row>
          <el-table
            :data="deviceStatus"
            style="width: 100%; font-size: x-small"
            :row-class-name="tableColor"
          >
            <el-table-column
              prop="alive"
              label="上报时间"
              min-width="150"
            ></el-table-column>
            <el-table-column
              prop="gps"
              label="当前位置"
              min-width="200"
            ></el-table-column>
            <el-table-column
              prop="alcohol"
              label="是否酒驾"
              min-width="70"
            ></el-table-column>
          </el-table>
          <el-row style="margin-top: 5%">
            <div class="amap-page-container">
              <el-amap
                vid="amapDemo"
                :center="mapCenter"
                :zoom="mapZoom"
                class="amap-demo"
              >
                <el-amap-marker
                  :position="currentMark.position"
                  :visible="currentMark.visible"
                  :draggable="currentMark.draggable"
                ></el-amap-marker>
              </el-amap>
            </div>
          </el-row>
        </el-row>
        <el-row style="margin-top: 6%, color: black">
          <el-button type="success" round="" @click="updateInfo()" color=black
            >跟踪</el-button
          >
        </el-row>
      </el-main>
    </el-container>
  </div>
</template>

<script>

import picture from '@/assets/1.png'

export default {
  name: "deviceIndex",
  data() {
    return {
      styles: {
        width: '1300px',
        height: '300px',
        backgroundImage: `url(${picture})`
      },
      msg: "Welcome to use ",
      timer: "",
      deviceNum: [],
      deviceId: 1,
      deviceStatus: [{ alive: "", gps: "", alcohol: "" }],
      mapCenter: [120.083711, 30.302106],
      mapZoom: 16,
      currentMark: {
        position: [120.083711, 30.302106],
        visible: true,
        draggable: false,
      },
      tableColor: "",
    };
  },
  methods: {
    updateInfo() {
      this.$axios
        .post("http://8.140.9.53:3389/device/getstatus")
        .then((res) => {
          console.log(res);
          this.deviceStatus[0].alcohol = res.data.alcohol == 1 ? "是" : "否";
          this.deviceStatus[0].gps = this.gpsCorrect(res.data.gps)
          //this.deviceStatus[0].gps = res.data.gps
          this.currentMark.position = this.deviceStatus[0].gps
          this.mapCenter = this.deviceStatus[0].gps
          this.getAddress(this.currentMark.position)
          let t = Date().toLocaleString()
          this.deviceStatus[0].alive = t.slice(0,24)
        })
        .catch(function (error) {
          console.log(error);
        });
    },
    timer() {
      return setTimeout(()=> {
        this.updateInfo() 
      },10000)
    },
    getAddress(coord){
      var geocoder = new AMap.Geocoder({
        radius: 10,
        extensions: "all"
      })
      geocoder.getAddress(coord, function(status, result) {
        if (status === 'complete' && result.info === 'OK') {
          if (result && result.regeocode) {
            localStorage.setItem("currentAddress",result.regeocode.formattedAddress)
          }
        }
      })
      this.deviceStatus[0].gps = localStorage.getItem("currentAddress")
    },
    gpsCorrect(coord){
      AMap.convertFrom(coord,"gps",function(status,result){
        localStorage.setItem("currentLng",result.locations[0].lng)
        localStorage.setItem("currentLat",result.locations[0].lat)
      })
      var correctCoord = []
      correctCoord.push(Number(localStorage.getItem("currentLng")))
      correctCoord.push(Number(localStorage.getItem("currentLat")))
      return correctCoord
    }
  },
  watch: {
      
  },
  mounted() {
      //this.timer = setTimeout(this.updateInfoTimer, 1000);
  },
};
</script>

<style>
.el-table .warning-row {
  background: rgb(7, 7, 7) !important;
}
</style>
<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped="">
h1,
h2 {
  font-weight: normal;
}
ul {
  list-style-type: none;
  padding: 0;
}
li {
  display: inline-block;
  margin: 0 10px;
}
a {
  color: #0d0e0d;
}
.amap-demo {
  height: 300px;
}
</style>
