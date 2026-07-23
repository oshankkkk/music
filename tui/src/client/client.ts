const socket = await Bun.connect({
  unix: "../../../build/us.socket",

  socket: {
    data(socket, data) {
      console.log("Server:", data.toString());
    },

    open(socket) {
      console.log("Connected!");

      socket.write("Hello from Bun client");
    },

    close(socket) {
      console.log("Disconnected");
    },

    error(socket, error) {
      console.error("Socket error:", error);
    },
  },
});
