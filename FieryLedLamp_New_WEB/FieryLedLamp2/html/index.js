$(document).ready(function(){
	bulmaSlider.attach();
});
function on_power()
{
	$.ajax({url:"/effect", data: {"power": true}, success: function(result){
		console.log(result);
	}});
};
function change_effect(selectObject)
{
	var value=selectObject.value;  
	console.log(value);
};
function on_provider_change()
{
	var provider=$("#provider").val();
	if(provider=="dealgate")
	{
		$("#server").val("mqtt.dealgate.ru");
		$("#port").val(1883);
	}
	if(provider=="homeassistant")
	{
		$("#port").val(1883);
		$("#discovery").val("homeassistant");
		$("#component").val("sensor");
	}
	var available_topic = $("#discovery").val()+"/"+$("#component").val()+"/"+$("#clientid").val()+"/available";
	var config_topic = $("#discovery").val()+"/"+$("#component").val()+"/"+$("#clientid").val()+"/config";
	var command_topic = $("#discovery").val()+"/"+$("#component").val()+"/"+$("#clientid").val()+"/command";

	$("#topic").html(available_topic+"<br>"+config_topic);
};