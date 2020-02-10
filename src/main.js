import Vue from "vue";
import App from "./App.vue";
import "./assets/tailwind.css";
import "./assets/colorpicker.css";
import ColorPicker from "@radial-color-picker/vue-color-picker";

Vue.config.productionTip = false;

Vue.use(ColorPicker);

new Vue({
  render: h => h(App)
}).$mount("#app");
