var app = new Vue({
    el: '#app',
    data: {
        data: [

        ]
    },
    created: function () {
        setInterval(this.fetchData, 5000)
    },
    methods: {
        fetchData: function () {
            const Http = new XMLHttpRequest();
            const url = '/data';
            Http.open("GET", url);
            Http.send();

            Http.onreadystatechange = (e) => {
                let data = Http.responseText
                try {
                    data = JSON.parse(data)
                    this.data = data
                } catch (err) {

                }
            }
        }
    }
})