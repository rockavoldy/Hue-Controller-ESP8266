<template>
  <div>
    <div class="w-100 border border-gray-400 rounded bg-white z-10">
      <div
        class="h-64 flex justify-center items-center text-white rounded-t"
        v-bind:style="{ background: color }"
      >
        <div class="text-center">
          <span class="text-white text-lg">{{ colorString }}</span>
        </div>
      </div>
      <div class="px-3 py-2 rounded-b">
        <div class="w-100 h-8 rounded mb-1" v-bind:style="gradH">
          <input
            :disabled="disabled"
            class="w-full py-2"
            type="range"
            min="0"
            max="360"
            v-model="h"
          />
        </div>
        <div class="w-100 h-8 rounded mb-1" v-bind:style="gradS">
          <input
            :disabled="disabled"
            class="w-full py-2"
            type="range"
            min="0"
            max="100"
            v-model="s"
          />
        </div>
        <div class="w-100 h-8 rounded mb-1" v-bind:style="gradL">
          <input
            :disabled="disabled"
            class="w-full py-2"
            type="range"
            min="0"
            max="100"
            v-model="l"
          />
        </div>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  props: ["change", "initial", "disabled"],
  data() {
    return {
      h: 150,
      s: 85,
      l: 0
    };
  },
  computed: {
    color: function() {
      let hsl = this.hsb2hsl(
        parseFloat(this.h) / 360,
        parseFloat(this.s) / 100,
        parseFloat(this.l) / 100
      );
      let c = hsl.h + ", " + hsl.s + "%, " + hsl.l + "%";
      let s = "hsl(" + c + ")";
      this.change({
        color: {
          h: this.h / (360 / 255),
          s: this.s / (100 / 255),
          v: this.l / (100 / 255)
        }
      });
      return s;
    },
    colorString: function() {
      var c = this.h + ", " + this.s + "%, " + this.l + "%";
      return c;
    },
    gradH: function() {
      var stops = [];
      for (var i = 0; i < 7; i++) {
        var h = i * 60;

        var hsl = this.hsb2hsl(
          parseFloat(h / 360),
          parseFloat(this.s) / 100,
          parseFloat(this.l / 100)
        );

        var c = hsl.h + ", " + hsl.s + "%, " + hsl.l + "%";
        stops.push("hsl(" + c + ")");
      }

      return {
        backgroundImage: "linear-gradient(to right, " + stops.join(", ") + ")"
      };
    },
    gradS: function() {
      var stops = [];
      var c;
      var hsl = this.hsb2hsl(
        parseFloat(this.h / 360),
        0,
        parseFloat(this.l / 100)
      );
      c = hsl.h + ", " + hsl.s + "%, " + hsl.l + "%";
      stops.push("hsl(" + c + ")");

      hsl = this.hsb2hsl(parseFloat(this.h / 360), 1, parseFloat(this.l / 100));
      c = hsl.h + ", " + hsl.s + "%, " + hsl.l + "%";
      stops.push("hsl(" + c + ")");

      return {
        backgroundImage: "linear-gradient(to right, " + stops.join(", ") + ")"
      };
    },

    gradL: function() {
      var stops = [];
      var c;

      var hsl = this.hsb2hsl(parseFloat(this.h / 360), 0, 0);
      c = hsl.h + ", " + hsl.s + "%, " + hsl.l + "%";
      stops.push("hsl(" + c + ")");

      hsl = this.hsb2hsl(parseFloat(this.h / 360), parseFloat(this.s / 100), 1);

      c = hsl.h + ", " + hsl.s + "%, " + hsl.l + "%";
      stops.push("hsl(" + c + ")");

      return {
        backgroundImage: "linear-gradient(to right, " + stops.join(", ") + ")"
      };
    }
  },
  methods: {
    hsb2hsl(h, s, b) {
      var hsl = {
        h: h
      };
      hsl.l = (2 - s) * b;
      hsl.s = s * b;

      if (hsl.l <= 1 && hsl.l > 0) {
        hsl.s /= hsl.l;
      } else {
        hsl.s /= 2 - hsl.l;
      }

      hsl.l /= 2;

      if (hsl.s > 1) {
        hsl.s = 1;
      }

      if (!hsl.s > 0) hsl.s = 0;

      hsl.h *= 360;
      hsl.s *= 100;
      hsl.l *= 100;

      return hsl;
    }
  }
};
</script>

<style></style>
