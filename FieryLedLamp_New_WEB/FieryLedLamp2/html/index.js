$(document).ready(function(){
	bulmaSlider.attach();
});
function on_power()
{
	console.log("power");
	$.get("power", function(data, status){
		console.log(status);
	});
};
function change_effect(selectObject)
{
	var value=selectObject.value;  
	console.log(value);
};