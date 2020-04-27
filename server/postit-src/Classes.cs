using System;
using System.ComponentModel.DataAnnotations;

public class Submission
{
    [Required]
    [StringLength(50, ErrorMessage = "Bro, pick a shorter game name.")]
    [RegularExpression("^[^']+$", ErrorMessage = "Don't use quotes; that'll break the browser.")]
    public string Name { get; set; }

    [Required]
    [StringLength(50, ErrorMessage = "Your hostname is too thicc. Shorten it.")]
    [RegularExpression("^[^']+$", ErrorMessage = "Don't use quotes; that'll break the browser.")]
    public string Host { get; set; }

    [Required]      // might be cool to add some regex to validate here..
    public string Code { get; set; }
}
