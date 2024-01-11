$(document).ready(function(){
	bulmaSlider.attach();
	
	$.ajax({url:"/effect.json", success: function(result){
		console.log(result);
		$("#effectList").clear();
	}});
});
function on_power()
{
	$.ajax({url:"power", success: function(result){
		console.log(result);
	}});
};
function change_effect(selectObject)
{
	var value=selectObject.value;  
	console.log(value);
};