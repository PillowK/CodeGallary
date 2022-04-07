# Create certificate
$mycert = New-SelfSignedCertificate -DnsName "인증서이름.com" -CertStoreLocation "cert:\CurrentUser\My" -NotAfter (Get-Date).AddYears(98) -KeySpec KeyExchange

# Export certificate to .pfx file
$mycert | Export-PfxCertificate -FilePath "인증서경로\mycert.pfx" -Password $(ConvertTo-SecureString -String "P@ssw0Rd1234" -AsPlainText -Force)

# Export certificate to .cer file
$mycert | Export-Certificate -FilePath "인증서경로\mycert.cer"

# Assign API permissions to the application

The procedures in this section replace any default permissions that were automatically configured for the new app. The app doesn't need the default permissions that were replaced.
On the app page under Management, select Manifest.
Select Manifest on the application properties page.
On the Manifest page that opens, find the requiredResourceAccess entry (on or about line 44).
Modify the resourceAppId, resourceAccess, id, and type values as shown in the following code snippet:

JSON Copy

"requiredResourceAccess": [
   {
      "resourceAppId": "00000002-0000-0ff1-ce00-000000000000",
      "resourceAccess": [
         {
            "id": "dc50a0fb-09a3-484d-be87-e023b12c6440",
            "type": "Role"
         }
      ]
   }
],
When you're finished, click Save.

# Assign Azure AD roles to the application

1. On the Azure AD portal at https://portal.azure.com/, under Manage Azure Active Directory, click View.
View in the Azure AD portal under Manage Azure Active Directory.

2. On the Overview page that opens, under Manage, select Roles and administrators.

3. Select Roles and administrators from the overview page.

4. On the Roles and administrators page that opens, find and select one of the supported roles by clicking on the name of the role (not the check box) in the results.

Exchange Online PowerShell:

Find and select a supported Exchange Online PowerShell role by clicking on the role name.

Security & Compliance Center PowerShell:

Find and select a supported Security & Compliance Center PowerShell role by clicking on the role name.

On the Assignments page that opens, click Add assignments.

Exchange Online PowerShell:

Select Add assignments on the role assignments page for Exchange Online PowerShell.

Security & Compliance Center PowerShell:

Select Add assignments on the role assignments page for Security & Compliance Center PowerShell.

In the Add assignments flyout that opens, find and select the app that you created in Step 1.

Find and select your app on the Add assignments flyout.

When you're finished, click Add.