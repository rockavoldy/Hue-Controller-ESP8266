<template>
  <div id="app">
    <div class="container mx-auto flex justify-center h-screen">
      <div class="self-center border w-6/12 text-center">
        <ColorPicker v-bind="color" @input="onInput"></ColorPicker>
        <pre>{{ color }}</pre>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  name: "App",
  data() {
    return {
      color: {
        hue: 20,
        saturation: 100,
        luminosity: 50,
        alpha: 1
      }
    };
  },
  methods: {
    onInput: function(hue) {
      this.color.hue = hue;
    }
  }
};
</script>
