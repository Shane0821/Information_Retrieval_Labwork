<template>
  <v-container>
    <v-card elevation="2" width="80%" class="mx-auto mb-6">
      <v-img
        height="250"
        src="https://img2.baidu.com/it/u=558331975,2258709620&fm=253&fmt=auto&app=138&f=JPEG?w=888&h=500"
      ></v-img>
      <!-- 模型 -->
      <v-card-title>
        查询模型
        <v-btn-toggle
          v-model="mod"
          mandatory
          dense
          group
          @change="expression = ''"
        >
          <v-btn class="ml-4">
            bool
          </v-btn>
          <v-btn class="ml-4">
            vector
          </v-btn>
          <v-btn class="ml-4">
            language
          </v-btn>
        </v-btn-toggle>
      </v-card-title>
      <!-- 查询内容 -->
      <v-row class="pl-3 pr-3">
        <v-card-title>
          查询内容
        </v-card-title>
        <v-text-field
            hide-details
            clearable
            v-model="expression"
        ></v-text-field>
        <v-btn icon class="mt-3" @click="query">
          <v-icon>
            mdi-magnify
          </v-icon>
        </v-btn>
      </v-row>
      <!-- 查询位置 -->
      <v-card-title v-if="mod === 0">
        查询位置
        <v-btn-toggle
          v-model="position"
          mandatory
          dense
          group
        >
          <v-btn class="ml-4">
            body
          </v-btn>
          <v-btn class="ml-4">
            title
          </v-btn>
          <v-btn class="ml-4">
            both
          </v-btn>
        </v-btn-toggle>
      </v-card-title>
    </v-card>
    <v-divider width="90%" class="mx-auto"></v-divider>
    <!-- 查询结果 -->
    <v-card elevation="2" width="80%" class="mx-auto mt-4">
      <v-expansion-panels multiple>
        <v-expansion-panel
          v-for="(item, index) in result"
          :key="index"
        >
          <v-expansion-panel-header>
            {{item.name}}：  {{item.score}}
          </v-expansion-panel-header>
          <v-expansion-panel-content class="text-justify">
              {{item.content}}
          </v-expansion-panel-content>
        </v-expansion-panel>
      </v-expansion-panels>
    </v-card>
    <v-snackbar
      v-model="snackbar"
      timeout="5000"
      top
      right
    >
      {{ errMessage }}
      <template v-slot:action="{ attrs }">
        <v-btn
          color="blue"
          text
          v-bind="attrs"
          @click="snackbar = false"
        >
          Close
        </v-btn>
      </template>
    </v-snackbar>
  </v-container>
</template>

<script>
import SearchServices from '@/services/SearchServices'

export default {
  name: 'HelloWorld',
  data () {
    return {
      position: 0,
      mod: 0,
      expression: '',
      errMessage: 'error',
      snackbar: false,
      result: []
    }
  },
  methods: {
    async query () {
      try {
        if (!this.expression) {
          this.result = []
          return
        }
        var pwdblank = /^\S*$/
        if (!pwdblank.test(this.expression)) {
          this.snackbar = true
          this.errMessage = '查询内容不能有空格！'
          this.result = []
          return
        }
        this.result = await SearchServices.index({
          expression: this.expression,
          position: this.position,
          mod: this.mod
        })
      } catch (error) {
        console.log(error)
        // this.errMessage = error.response.data.error
        this.snackbar = true
      }
    }
  }
}
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
h1, h2 {
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
  color: #42b983;
}

.v-expansion-panel-header {
  font-size: 1.2rem;
}
</style>
