(function ($) {
    $(function () {

        $('.button-collapse').sideNav();

    }); // end of document ready
})(jQuery); // end of jQuery name space
//---------------------------------------------------------------------------------------



//---------------------------------------------------------------------------------------
// Global
var username = 'Jose Ramon Navarro Marquez';
var devices = [];

//---------------------------------------------------------------------------------------
var app = angular.module('iotApp', ["ngRoute",'chart.js','nvd3']);

app.controller('mainCtrl', function ($scope,$window) {
    $scope.labels = ["January", "February", "March", "April", "May", "June", "July"];
    $scope.series = ['Series A', 'Series B'];
    $scope.data = [
        [65, 59, 80, 81, 56, 55, 40],
        [28, 48, 40, 19, 86, 27, 90]
    ];
    $scope.onClick = function (points, evt) {
        console.log(points, evt);
    };
    $scope.datasetOverride = [{ yAxisID: 'y-axis-1' }, { yAxisID: 'y-axis-2' }];
    $scope.options = {
        scales: {
            yAxes: [
                {
                    id: 'y-axis-1',
                    type: 'linear',
                    display: true,
                    position: 'left'
                },
                {
                    id: 'y-axis-2',
                    type: 'linear',
                    display: true,
                    position: 'right'
                }
            ]
        }
    };
});

app.controller('tutorialCtrl', function ($scope) {
});

app.controller('dashboardCtrl', function ($scope) {
});

app.controller('dipositivosCtrl', function ($scope) {

    var optionsDef = {
        chart: {
            type: 'lineWithFocusChart',
            height: 450,
            margin : {
                top: 20,
                right: 20,
                bottom: 60,
                left: 40
            },
            duration: 500,
            useInteractiveGuideline: true,
            xAxis: {
                axisLabel: 'X Axis',
                tickFormat: function(d){
                    return d3.format(',f')(d);
                }
            },
            x2Axis: {
                tickFormat: function(d){
                    return d3.format(',f')(d);
                }
            },
            yAxis: {
                axisLabel: 'Y Axis',
                tickFormat: function(d){
                    return d3.format(',.2f')(d);
                },
                rotateYLabel: false
            },
            y2Axis: {
                tickFormat: function(d){
                    return d3.format(',.2f')(d);
                }
            }

        }
    };

    var dataDef = generateData();

    /* Random Data Generator (took from nvd3.org) */
    function generateData() {
        return stream_layers(3,10+Math.random()*200,.1).map(function(data, i) {
            return {
                key: 'Stream' + i,
                values: data
            };
        });
    }

    /* Inspired by Lee Byron's test data generator. */
    function stream_layers(n, m, o) {
        if (arguments.length < 3) o = 0;
        function bump(a) {
            var x = 1 / (.1 + Math.random()),
                y = 2 * Math.random() - .5,
                z = 10 / (.1 + Math.random());
            for (var i = 0; i < m; i++) {
                var w = (i / m - y) * z;
                a[i] += x * Math.exp(-w * w);
            }
        }
        return d3.range(n).map(function() {
            var a = [], i;
            for (i = 0; i < m; i++) a[i] = o + o * Math.random();
            for (i = 0; i < 5; i++) bump(a);
            return a.map(stream_index);
        });
    }

    function stream_index(d, i) {
        return {x: i, y: Math.max(0, d)};
    }

    //------------------------------------------------------------------------------------------------------------------

    $scope.notas = [
        {titulo: 'Foco 1', contenido: "Contenido de la nota 1", options: optionsDef, data:generateData()},
        {titulo: 'Foco 2', contenido: "Contenido de la nota 2", options: optionsDef, data:generateData()}
    ];

    $scope.addNote = function () {
        $scope.notas.push({text: $scope.todoText, done: false});
        $scope.todoText = '';
    };

    $scope.remaining = function () {
        var count = 0;
        angular.forEach($scope.notas, function (todo) {
            count++;
        });
        return count;
    };

    $scope.archive = function () {
        var oldTodos = $scope.notas;
        $scope.notas = [];
        angular.forEach(oldTodos, function (todo) {
            //if (!todo.done) notasList.notas.push(todo);
        });
    };
});
app.config(function ($routeProvider,$locationProvider) {
    $routeProvider
        .when("/", {
            templateUrl: "pages/tutorial.html",
            controller: "tutorialCtrl"
        })
        .when("/dashboard", {
            templateUrl: "pages/dashboard.html",
            controller: "dashboardCtrl"
        })
        .when("/dispositivos", {
            templateUrl: "pages/dispositivos.html",
            controller: "dipositivosCtrl"
        })
        .when("/eventos", {
            templateUrl: "pages/eventos.html",
            controller: "eventosCtrl"
        })
        .when("/configuracion", {
            templateUrl: "pages/configuracion.html",
            controller: "eventosCtrl"
        });

    $routeProvider.otherwise( { redirectTo: '/' });

    $locationProvider.hashPrefix('!');
});

