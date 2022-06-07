import Api from '@/services/Api'

export default {
  index (data) {
    return Api().get('/', {
      params: {
        expression: data.expression,
        mod: data.mod,
        position: data.position
      }
    })
  }
}
