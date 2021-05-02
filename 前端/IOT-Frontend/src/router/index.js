import Vue from 'vue'
import Router from 'vue-router'
import deviceIndex from '@/components/deviceIndex'

Vue.use(Router)

export default new Router({
  routes: [
    {
      path: '/',
      name: 'deviceIndex',
      component: deviceIndex
    }
  ]
})
