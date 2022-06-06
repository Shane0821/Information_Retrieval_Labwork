import Api from '@/services/Api'

export default {
  index (data) {
    console.log(data.expression)
    return Api().get('/', {
      params: {
        expression: data.expression,
        mod: data.mod,
        position: data.position
      }
    })
  }
}
