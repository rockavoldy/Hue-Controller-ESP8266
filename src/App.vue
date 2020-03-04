<template>
  <div id="app">
    <div class="container mx-auto flex  flex-wrap h-screen">
      <div class="w-full h-20 text-center mt-4 lg:mt-20">
        <div v-if="connected">
          <h1 class="text-lg">Connected</h1>
          <div class="rounded bg-gray-400 px-1 py-1 inline-block">
            <span>
              <input
                :disabled="connected"
                class="rounded-l bg-gray-200 px-2 py-1 border border-gray-200"
                v-model="url"
                type="text"
                placeholder="ws://192.168.100.15:8000"
            /></span>
            <button
              class="px-2 py-1 rounded-r bg-red-700 border border-red-700 text-white"
              @click="disconnectWS()"
            >
              Disconnect
            </button>
          </div>
        </div>
        <div v-else>
          <h1 class="text-lg">Disconnected</h1>
          <div class="rounded bg-gray-400 px-1 py-1 inline-block">
            <span>
              <input
                class="rounded-l bg-gray-200 px-2 py-1 border border-gray-200 hover:bg-gray-100 hover:underline focus:bg-gray-100 focus:underline"
                v-model="url"
                type="text"
                placeholder="ws://192.168.100.15:8000"
            /></span>
            <button
              class="px-2 py-1 rounded-r bg-gray-700 border border-gray-700 text-white"
              @click="connectWebSocket()"
            >
              Connect
            </button>
          </div>
        </div>
        <br />
      </div>
      <div class="w-full flex justify-center">
        <ColorPicker
          class="w-10/12 sm:w-10/12 lg:w-6/12 xl:w-4/12"
          :disabled="!connected"
          :change="updateColor"
        />
      </div>
    </div>
  </div>
</template>

<script>
import ColorPicker from "./components/ColorPicker";
export default {
  name: "App",
  components: {
    ColorPicker
  },
  data() {
    return {
      url: "ws://192.168.100.15:8000",
      color: {
        h: 0,
        s: 0,
        v: 0
      },
      socket: null,
      connected: false,
      connecting: null
    };
  },
  created() {
    // this.connectWebSocket();
  },
  methods: {
    updateColor: function(event) {
      this.color = event.color;
      if (this.socket) {
        this.sendColor(event.color);
      }
    },
    connectWebSocket() {
      const socket = new WebSocket(this.url);
      socket.onclose = () => {
        if (socket.readyState === socket.CONNECTING) {
          this.connecting = setTimeout(() => {
            this.connectWebSocket();
          }, 1000);
        }
      };

      socket.onopen = () => {
        clearTimeout(this.connecting);
        this.socket = socket;
        this.connected = true;
      };
    },
    disconnectWS() {
      this.socket.close();
      this.connected = false;
    },
    sendColor(color) {
      const data = {
        h: color.h,
        s: color.s,
        v: color.v
      };
      this.socket.send(JSON.stringify(data));
    }
  }
};
</script>
