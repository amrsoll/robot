$('div .expand').click(function () {                  
    var checkedState =   $(this).prop("checked")
    $(this)
          .parent('div')
          .children('.expand:checked')
          .prop("checked", false);
    
    $(this).prop("checked", checkedState);
});