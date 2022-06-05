import Api from '@/services/Api'

export default {
  index (data) {
    // console.log(data)
    return Api().get('/', data)
  }
}
