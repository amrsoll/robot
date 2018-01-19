/**
 * @Author: Axel_Soll <amrsoll>
 * @Date:   12/01/2018
 * @Email:  axel.soll@telecom-paristech.fr
 * @Last modified by:   amrsoll
 * @Last modified time: 12/01/2018
 */



$('div .expand').click(function () {
    var checkedState =   $(this).prop("checked")
    $(this)
          .parent('div')
          .children('.expand:checked')
          .prop("checked", false);

    $(this).prop("checked", checkedState);
});

function show_scan_fail(){
    var img_scan_fail = document.getElementById('scan_fail');
	if(button.parentElement.classList.contains('hidden')){
		button.parentElement.classList.remove('hidden');
	} else {
	button.parentElement.classList.add('hidden');
	}
}
