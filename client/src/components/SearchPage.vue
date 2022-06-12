<template>
  <v-container>
    <v-card elevation="2" width="80%" class="mx-auto mb-6">
      <v-img
        height="250"
        src="https://img2.baidu.com/it/u=558331975,2258709620&fm=253&fmt=auto&app=138&f=JPEG?w=888&h=500"
      ></v-img>
      <!-- 模型 -->
      <v-card-title>
        查询类型
        <v-btn-toggle
          v-model="mod"
          mandatory
          dense
          group
          @change="()=>{expression = ''; searched = 0}"
        >
          <v-btn class="ml-4">
            bool
          </v-btn>
          <v-btn class="ml-4">
            Natural language
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
        <v-btn icon class="mt-3" @click="()=>{query()}">
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
      <v-banner
          v-if="result.data==undefined && searched"
          class="mx-auto"
      >无匹配结果</v-banner>
      <!-- 布尔查询的结果 -->
      <v-expansion-panels multiple v-if="result.data!=undefined && searched">
       <v-expansion-panel
         v-for="(item, index) in result.data['0']"
         :key="index"
       >
        <v-expansion-panel-header>
          <span>文档编号：{{item.id}}</span>
          <span>得分：{{item.score.toFixed(15)}}</span>
        </v-expansion-panel-header>
        <v-expansion-panel-content class="text-justify">
          <h3>{{item.title}}</h3>
          <span>{{item.content}}</span>
        </v-expansion-panel-content>
       </v-expansion-panel>
      </v-expansion-panels>
      <!-- 自然语言查询结果，支持三种模型对比 -->
      <v-card v-if="result.data!=null && mod === 1 && searched">
       <v-tabs
        v-model="tab"
        background-color="blue-grey lighten-1"
        height=50
        centered
        dark
        icons-and-text
       >
        <v-tabs-slider></v-tabs-slider>

        <v-tab href="#tab-1">
          vector
        </v-tab>

        <v-tab href="#tab-2">
          language
        </v-tab>

        <v-tab href="#tab-3">
          probabilistic
        </v-tab>
      </v-tabs>

      <v-tabs-items v-model="tab">
        <v-tab-item
          v-for="i in 3"
          :key="i"
          :value="'tab-' + i"
        >
         <v-expansion-panels multiple>
          <v-expansion-panel
            v-for="(item, index) in result.data[i.toString()]"
            :key="index"
          >
            <v-expansion-panel-header>
              <span>文档编号：{{item.id}}</span>
              <span>得分：{{item.score.toFixed(10)}}</span>
            </v-expansion-panel-header>
            <v-expansion-panel-content class="text-justify">
              <h3>{{item.title}}</h3>
              <span>{{item.content}}</span>
            </v-expansion-panel-content>
          </v-expansion-panel>
         </v-expansion-panels>
        </v-tab-item>
       </v-tabs-items>
      </v-card>
    </v-card>
    <v-snackbar
      v-model="snackbar"
      timeout="2500"
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
  name: 'SearchPage',
  data () {
    return {
      tab: null,
      searched: 0,
      position: 0,
      mod: 0,
      expression: '',
      errMessage: 'error',
      snackbar: false,
      result: {}
    }
  },
  methods: {
    async query () {
      try {
        if (!this.expression) {
          this.result = {}
          return
        }
        var pwdblank = /^\S*$/
        if (!pwdblank.test(this.expression)) {
          this.errMessage = '查询内容不能有空格！'
          this.snackbar = true
          this.result = {}
          return
        }
        this.result = await SearchServices.index({
          expression: this.expression,
          position: this.position,
          mod: this.mod
        })
        console.log(this.result)
        this.searched = 1
      } catch (error) {
        // console.log(error)
        this.errMessage = error.response.data
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
